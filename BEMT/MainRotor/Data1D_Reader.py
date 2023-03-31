# -*- coding: utf-8 -*-
"""
Created on Fri Mar  3 00:03:32 2023

@author: musta
"""

import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator
from math import pi
import numpy as np
from matplotlib.colors import LogNorm


def Data1DPlotter(filename,title):
    data = pd.read_csv(filename)
    xData = data["Azimuth Angle"] * 180 / pi
    fig, axes = plt.subplots(nrows=1, ncols=2)
    fig.suptitle(title,fontsize = 15, fontweight = "bold")
    axes[0].minorticks_on()
    axes[0].set_title("Flap")
    axes[0].grid(which = "major")
    axes[0].grid(which = "minor")
    yData = data["Flap"] * 180 / pi
    axes[0].plot(xData, yData)
    
    axes[1].minorticks_on()
    axes[1].set_title("PitchLinkMoment")
    axes[1].grid(which = "major")
    axes[1].grid(which = "minor")
    yData = data["PitchLinkMoment"]
    axes[1].plot(xData, yData)
    
    return
def Data2DPlotterColorMesh(filename,title):
    df = pd.read_csv(filename)
    NumberOfVariables = len(df.columns) - 2
    # Extract the columns
    radius_values = np.sort(df["Radius Location"].unique())
    azim_values = np.sort(df["Azimuth Angle"].unique())
    radius_grid, angle_grid = np.meshgrid(radius_values, azim_values)

    # reshape the data into a 2D grid
    dataList = []
    contourList = []
    titleList = []
    for col in df.iloc[:, 2:]:
        if col == "AoA":
            
            data_grid = df.pivot(index="Azimuth Angle", columns="Radius Location",
                               values=col).values * 180. / pi
        else:
            data_grid = df.pivot(index="Azimuth Angle", columns="Radius Location",
                                 values=col).values
        dataList.append(data_grid)
        contours = np.sort(data_grid[0][:])
        contourList.append(contours)
        titleList.append(col)
        
    
    # Calculate the number of rows and columns needed to create a square-like grid
    n_cols = int(np.ceil(np.sqrt(NumberOfVariables)))
    n_rows = int(np.ceil(NumberOfVariables / n_cols))
    
    # Calculate the figure size to maintain a fixed aspect ratio
    fig_size = (n_cols * 4, n_rows * 4)
    # plot the contour plot
    fig, axes = plt.subplots(n_rows,n_cols,
                           figsize=fig_size,
                           subplot_kw=dict(projection="polar"))
    fig.suptitle(title,fontsize = 15, fontweight = "bold")
    axes = axes.flatten()
    for index,data_grid in enumerate(dataList):
        levels = contourList[index]
        contour = axes[index].contourf(angle_grid, radius_grid, data_grid ,
                              cmap='RdYlGn',
                              vmin = np.min(data_grid), vmax = np.max(data_grid),
                              levels = 30)

        axes[index].set_theta_zero_location("S")
        # Label contours
        axes[index].grid(False,axis='y', which='both')
        axes[index].set_yticklabels([])
        axes[index].grid(True,axis='x', which='both',linewidth = 0.5, color = "red")
        axes[index].set_title(titleList[index],fontweight = "bold")
        fig.colorbar(contour, ax=axes[index], shrink=1)
    
    for ax in axes[NumberOfVariables:]:
        ax.axis("off")
    fig.tight_layout(rect=[0, 0, 1, 0.95])
    plt.show()
    return
def Data2DPlotterContour(filename,title):
    df = pd.read_csv(filename)
    NumberOfVariables = len(df.columns) - 2
    # Extract the columns
    radius_values = np.sort(df["Radius Location"].unique())
    azim_values = np.sort(df["Azimuth Angle"].unique())
    radius_grid, angle_grid = np.meshgrid(radius_values, azim_values)

    # reshape the data into a 2D grid
    dataList = []
    contourList = []
    titleList = []
    for col in df.iloc[:, 2:]:
        if col == "AoA":
            data_grid = df.pivot(index="Azimuth Angle", columns="Radius Location",
                               values=col).values * 180. / pi
        else:
            data_grid = df.pivot(index="Azimuth Angle", columns="Radius Location",
                                 values=col).values
        dataList.append(data_grid)
        contours = np.sort(data_grid[0][:] )
        contourList.append(contours)
        titleList.append(col)
        
    
    # Calculate the number of rows and columns needed to create a square-like grid
    n_cols = int(np.ceil(np.sqrt(NumberOfVariables)))
    n_rows = int(np.ceil(NumberOfVariables / n_cols))
    
    # Calculate the figure size to maintain a fixed aspect ratio
    fig_size = (n_cols * 4, n_rows * 4)
    # plot the contour plot
    fig, axes = plt.subplots(n_rows,n_cols,
                           figsize=fig_size,
                           subplot_kw=dict(projection="polar"))
    fig.suptitle(title,fontsize = 15, fontweight = "bold")
    axes = axes.flatten()
    for index,data_grid in enumerate(dataList):
        levels = contourList[index]
        contour = axes[index].contour(angle_grid, radius_grid, data_grid,colors = 'k' ,
                              cmap = None,levels = levels,linewidths = 2)
        axes[index].set_theta_zero_location("S")
        # Label contours
        axes[index].clabel(contour, inline=True, fontsize=10)
        # axes[index].grid(False)
        axes[index].grid(False,axis='y', which='both')
        axes[index].set_yticklabels([])
        axes[index].grid(True,axis='x', which='both',linewidth = 0.5, color = "red")
        axes[index].set_title(titleList[index],fontweight = "bold")
    
    for ax in axes[NumberOfVariables:]:
        ax.axis("off")
    fig.tight_layout(rect=[0, 0, 1, 0.95])
    plt.show()
    return

Data1DPlotter("Data_1D.dat","Flap And Pitch Link Moments")
Data2DPlotterContour("Data_2D.dat", "Aerodynamics Contours")
Data2DPlotterColorMesh("Data_2D.dat","Aerodynamics ColorPlot")
Data2DPlotterContour("ShaftLiftVec_2D.dat", "Shaft Lift Contours")
Data2DPlotterContour('SectionLiftVec_2D.dat','Section Lift Contours')
Data2DPlotterColorMesh('SectionLiftVec_2D.dat','Section Lift Contours')
